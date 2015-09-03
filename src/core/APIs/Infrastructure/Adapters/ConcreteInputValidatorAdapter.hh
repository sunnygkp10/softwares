<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Validators\Input\InputValidator;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Domain\Configurations\Variables\VariableAdapter;
use APIs\Infrastructure\Objects\ConcreteInputValidator;
use APIs\Domain\Configurations\Validators\Input\InputValidatorAdapter;

final class ConcreteInputValidatorAdapter implements InputValidatorAdapter {
    private $variableAdapter;
    private $validators;
    public function __construct(VariableAdapter $variableAdapter, Map<string, Validator> $validators) {
        $this->variableAdapter = $variableAdapter;
        $this->validators = $validators;
    }

    public function convertDataToInputValidator(Map<string, mixed> $data): InputValidator {

        if (!isset($data['input']) || !isset($data['validator'])) {
            throw new \Exception('The input and validator keynames are mandatory in order to convert data to an InputValidator object.');
        }

        if (!isset($this->validators[$data['validator']])) {
            throw new \Exception('The given validator ('.$data['validator'].') is not defined in the validators section.');
        }

        $variable = $this->variableAdapter->convertStringToVariable($data['input']);
        return new ConcreteInputValidator($variable, $this->validators[$data['validator']]);
    }

}

<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Validators\Input\InputValidator;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Domain\Configurations\Variables\Variable;

final class ConcreteInputValidator implements InputValidator {
    private $variable;
    private $validator;
    public function __construct(Variable $variable, Validator $validator) {
        $this->variable = $variable;
        $this->validator = $validator;
    }

    public function getVariable(): Variable {
        return $this->variable;
    }

    public function getValidator(): Validator {
        return $this->validator;
    }

}

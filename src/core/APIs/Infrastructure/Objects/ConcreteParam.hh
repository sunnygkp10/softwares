<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Params\Param;
use APIs\Domain\Configurations\Variables\Variable;
use APIs\Domain\Configurations\Validators\Input\InputValidator;
use APIs\Domain\Configurations\Rendered\RenderedValue;

final class ConcreteParam implements Param {
    private $value;
    private $variable;
    private $validator;
    private $renderedValue;
    public function __construct(mixed $value = null, Variable $variable = null, InputValidator $validator = null, RenderedValue $renderedValue = null) {

        $amount = (is_null($value) ? 0 : 1) + (is_null($variable) ? 0 : 1) + (is_null($validator) ? 0 : 1) + (is_null($renderedValue) ? 0 : 1);
        if ($amount != 1) {
            throw new \Exception('One of these must be given: value, variable, validator, renderedValue.  '.$amount.' given.');
        }

        $this->value = $value;
        $this->variable = $variable;
        $this->validator = $validator;
        $this->renderedValue = $renderedValue;

    }

    public function isValue(): boolean {
        return !is_null($this->value);
    }

    public function getValue(): ?mixed {
        return $this->value;
    }

    public function isVariable(): boolean {
        return !is_null($this->variable);
    }

    public function getVariable(): ?Variable {
        return $this->variable;
    }

    public function isInputValidator(): boolean {
        return !is_null($this->validator);
    }

    public function getInputValidator(): ?InputValidator {
        return $this->validator;
    }

    public function isRenderedValue(): boolean {
        return !empty($this->renderedValue);
    }

    public function getRenderedValue(): RenderedValue {
        return $this->renderedValue;
    }

}

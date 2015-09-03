<?hh
namespace APIs\Domain\Configurations\Validators\Input;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Domain\Configurations\Variables\Variable;

interface InputValidator {
    public function getVariable(): Variable;
    public function getValidator(): Validator;
}

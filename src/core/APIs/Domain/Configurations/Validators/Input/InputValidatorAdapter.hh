<?hh
namespace APIs\Domain\Configurations\Validators\Input;
use APIs\Domain\Configurations\Validators\Input\InputValidator;

interface InputValidatorAdapter {
    public function convertDataToInputValidator(Map<string, mixed> $data): InputValidator;
}

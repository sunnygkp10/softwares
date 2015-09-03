<?hh
namespace APIs\Domain\Configurations\Validators;
use APIs\Domain\Configurations\Functions\FunctionCode;

interface Validator {
    public function getFunctionCode(): FunctionCode;
    public function hasSubValidators(): boolean;
    public function getSubValidators(): ?Map<string, Validator>;
}

<?hh
namespace APIs\Domain\Configurations\Params;
use APIs\Domain\Configurations\Validators\Input\InputValidator;
use APIs\Domain\Configurations\Variables\Variable;
use APIs\Domain\Configurations\Rendered\RenderedValue;

interface Param {
    public function isValue(): boolean;
    public function getValue(): ?mixed;
    public function isVariable(): boolean;
    public function getVariable(): ?Variable;
    public function isInputValidator(): boolean;
    public function getInputValidator(): ?InputValidator;
    public function isRenderedValue(): boolean;
    public function getRenderedValue(): RenderedValue;
}

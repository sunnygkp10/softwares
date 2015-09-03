<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

final class ConcreteValidator implements Validator {
    private $code;
    private $subValidators;
    private $view;
    private $renderedParser;
    public function __construct(FunctionCode $code, Map<string, Validator> $subValidators = null, View $view = null, RenderedParser $renderedParser = null) {

        if (empty($subValidators)) {
            $subValidators = null;
        }

        $this->code = $code;
        $this->subValidators = $subValidators;
        $this->view = $view;
        $this->renderedParser = $renderedParser;
    }

    public function getFunctionCode(): FunctionCode {
        return $this->code;
    }

    public function hasSubValidators(): boolean {
        return !empty($this->subValidators);
    }

    public function getSubValidators(): ?Map<string, Validator> {
        return $this->subValidators;
    }

    public function hasView() {
        return !empty($this->view);
    }

    public function getView() {
        return $this->view;
    }

    public function hasRenderedParser() {
        return !empty($this->renderedParser);
    }

    public function getRenderedParser() {
        return $this->renderedParser;
    }

}

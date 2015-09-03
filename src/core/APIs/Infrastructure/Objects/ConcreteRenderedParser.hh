<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;
use APIs\Domain\Configurations\Functions\FunctionCode;

final class ConcreteRenderedParser implements RenderedParser {
    private $code;
    public function __construct(FunctionCode $code) {
        $this->code = $code;
    }

    public function getFunctionCode(): FunctionCode {
        return $this->code;
    }

}

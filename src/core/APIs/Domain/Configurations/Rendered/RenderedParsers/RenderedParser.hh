<?hh
namespace APIs\Domain\Configurations\Rendered\RenderedParsers;
use APIs\Domain\Configurations\Functions\FunctionCode;

interface RenderedParser {
    public function getFunctionCode(): FunctionCode;
}

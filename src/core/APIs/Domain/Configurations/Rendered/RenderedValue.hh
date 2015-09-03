<?hh
namespace APIs\Domain\Configurations\Rendered;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

interface RenderedValue {
    public function getValue(): string;
    public function getRenderedParser(): RenderedParser;
}

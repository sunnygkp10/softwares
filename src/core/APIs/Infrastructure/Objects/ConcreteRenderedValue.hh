<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Rendered\RenderedValue;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

final class ConcreteRenderedValue implements RenderedValue {
    private $value;
    private $renderedParser;
    public function __construct(string $value, RenderedParser $renderedParser) {
        $this->value = $value;
        $this->renderedParser = $renderedParser;
    }

    public function getValue(): string {
        return $this->value;
    }

    public function getRenderedParser(): RenderedParser {
        return $this->renderedParser;
    }

}

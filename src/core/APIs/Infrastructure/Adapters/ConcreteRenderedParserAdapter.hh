<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParserAdapter;
use APIs\Infrastructure\Objects\ConcreteRenderedParser;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;

final class ConcreteRenderedParserAdapter implements RenderedParserAdapter {
    private $functionCodeAdapter;
    public function __construct(FunctionCodeAdapter $functionCodeAdapter) {
        $this->functionCodeAdapter = $functionCodeAdapter;
    }

    public function convertRawConfigurationToRenderedParsers(RawConfiguration $configuration): Map<string, RenderedParser> {

        $renderedParsers = $configuration->get('rendered_parsers');
        $functionCodes = $this->functionCodeAdapter->convertDataToFunctionCodes($renderedParsers);

        $output = Map<string, RenderedParser> {};
        foreach($functionCodes as $keyname => $oneFunctionCode) {
            $output[$keyname] = new ConcreteRenderedParser($oneFunctionCode);
        }

        return $output;
    }

}

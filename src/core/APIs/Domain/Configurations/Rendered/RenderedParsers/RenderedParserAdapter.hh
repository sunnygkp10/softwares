<?hh
namespace APIs\Domain\Configurations\Rendered\RenderedParsers;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

interface RenderedParserAdapter {
    public function convertRawConfigurationToRenderedParsers(RawConfiguration $configuration): Map<string, RenderedParser>;
}

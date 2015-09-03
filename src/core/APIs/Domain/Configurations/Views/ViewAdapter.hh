<?hh
namespace APIs\Domain\Configurations\Views;
use APIs\Domain\Configurations\RawConfiguration;

interface ViewAdapter {
    public function convertRawConfigurationToViews(RawConfiguration $configuration): Map<string, View>;
}

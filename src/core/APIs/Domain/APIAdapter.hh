<?hh
namespace APIs\Domain;
use APIs\Domain\Configurations\RawConfiguration;

interface APIAdapter {
    public function convertRawConfigurationToAPI(RawConfiguration $configuration): API;
}

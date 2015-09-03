<?hh
namespace APIs\Domain\Configurations\RESTAPIs;
use APIs\Domain\Configurations\RawConfiguration;

interface RESTAPIAdapter {
    public function convertRawConfigurationToRESTAPIs(RawConfiguration $configuration): Map<string, RESTAPI>;
}

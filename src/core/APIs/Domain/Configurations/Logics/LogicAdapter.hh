<?hh
namespace APIs\Domain\Configurations\Logics;
use APIs\Domain\Configurations\RawConfiguration;

interface LogicAdapter {
    public function convertRawConfigurationToLogics(RawConfiguration $configuration): Map<string, Logic>;
}

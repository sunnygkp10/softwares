<?hh
namespace APIs\Domain\Configurations\Routes;
use APIs\Domain\Configurations\RawConfiguration;

interface RouteAdapter {
    public function convertRawConfigurationToRoutes(RawConfiguration $configuration): Vector<Route>;
}

<?hh
namespace APIs\Domain\Configurations\Routes;
use APIs\Domain\Configurations\Routes\Https\HttpRoute;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;
use APIs\Domain\Configurations\Routes\CLI\CLIRoute;

interface Route {
    public function isHttpRoute(): boolean;
    public function getHttpRoute(): ?HttpRoute;
    public function isCLIRoute(): boolean;
    public function getCLIRoute(): ?CLIRoute;
    public function getRESTAPI(): RESTAPI;
}

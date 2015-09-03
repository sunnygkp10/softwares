<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Routes\Route;
use APIs\Domain\Configurations\Routes\Https\HttpRoute;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;
use APIs\Domain\Configurations\Routes\CLI\CLIRoute;

final class ConcreteRoute implements Route {
    private $api;
    private $httpRoute;
    private $cliRoute;
    public function __construct(RESTAPI $api, HttpRoute $httpRoute = null, CLIRoute $cliRoute = null) {

        $amount = (!empty($httpRoute) ? 1 : 0) + (!empty($cliRoute) ? 1 : 0);
        if ($amount != 1) {
            throw new \Exception('One of these must be non-null: httpRoute, cliROute.  '.$amount.' given.');
        }

        $this->api = $api;
        $this->httpRoute = $httpRoute;
        $this->cliRoute = $cliRoute;
    }

    public function isHttpRoute(): boolean {
        return !empty($this->httpRoute);
    }

    public function getHttpRoute(): ?HttpRoute {
        return $this->httpRoute;
    }

    public function isCLIRoute(): boolean {
        return !empty($this->cliRoute);
    }

    public function getCLIRoute(): ?CLIRoute {
        return $this->cliRoute;
    }

    public function getRESTAPI(): RESTAPI {
        return $this->api;
    }

}

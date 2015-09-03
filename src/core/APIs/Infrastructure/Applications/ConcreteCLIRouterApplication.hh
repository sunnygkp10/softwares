<?hh
namespace APIs\Infrastructure\Applications;
use APIs\Applications\Routers\CLIRouterApplication;
use APIs\Applications\RESTAPIs\RESTAPIApplication;
use APIs\Infrastructure\Objects\ConcreteExecutableRESTAPI;
use APIs\Domain\API;

final class ConcreteCLIRouterApplication implements CLIRouterApplication {
    private $api;
    private $restAPIApplication;
    public function __construct(API $api, RESTAPIApplication $restAPIApplication) {
        $this->api = $api;
        $this->restAPIApplication = $restAPIApplication;
    }

    public function route() {

        $cliParams = Map<string, mixed> {
            'cli' => Map<string, string> {}
        };

        if ($this->api->hasCLI()) {
            $cli = $this->api->getCLI();
            foreach($cli as $keyname => $globalKeyname) {
                if (isset($GLOBALS[$globalKeyname])) {
                    $cliParams['cli'][$keyname] = $GLOBALS[$globalKeyname];
                }
            }
        }

        $routes = $this->api->getRoutes();
        foreach($routes as $index => $oneRoute) {

            if (!$oneRoute->isCLIRoute()) {
                continue;
            }

            $cliRoute = $oneRoute->getCLIRoute();
            $restParams = Map<string, string> {};
            if ($cliRoute->hasMandatoryParams()) {

                try {

                    $mandatoryParamVariables = $cliRoute->getMandatoryParams();
                    foreach($mandatoryParamVariables as $keyname => $oneVariable) {
                        $restParams[$keyname] = $oneVariable->fetch($cliParams);
                    }

                } catch (\Exception $exception) {
                    continue;
                }
            }

            if ($cliRoute->hasOptionalParams()) {

                try {

                    $optionalParamVariables = $cliRoute->getOptionalParams();
                    foreach($optionalParamVariables as $keyname => $oneVariable) {
                        $restParams[$keyname] = $oneVariable->fetch($cliParams);
                    }

                } catch (\Exception $exception) {

                }

            }

            $restAPI = $oneRoute->getRESTAPI();
            $executableRESTAPI = new ConcreteExecutableRESTAPI($restAPI, $restParams);
            echo $this->restAPIApplication->execute($executableRESTAPI);
            return;

        }

        throw new \Exception('There was no route found that matched the request.', 404);

    }

}

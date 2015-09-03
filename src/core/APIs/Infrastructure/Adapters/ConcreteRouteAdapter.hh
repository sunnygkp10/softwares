<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Configurations\Routes\RouteAdapter;
use APIs\Domain\Configurations\Routes\Route;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;
use APIs\Infrastructure\Objects\ConcreteRoute;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePatternAdapter;
use APIs\Infrastructure\Objects\ConcreteHttpRoute;
use APIs\Infrastructure\Objects\ConcreteCLIRoute;
use APIs\Domain\Configurations\Variables\VariableAdapter;
use APIs\Domain\Configurations\Variables\Variable;

final class ConcreteRouteAdapter implements RouteAdapter {
    private $httpRoutePatternAdapter;
    private $variableAdapter;
    private $restAPIs;
    public function __construct(HttpRoutePatternAdapter $httpRoutePatternAdapter, VariableAdapter $variableAdapter, Map<string, RESTAPI> $restAPIs) {
        $this->httpRoutePatternAdapter = $httpRoutePatternAdapter;
        $this->variableAdapter = $variableAdapter;
        $this->restAPIs = $restAPIs;
    }

    public function convertRawConfigurationToRoutes(RawConfiguration $configuration): Vector<Route> {

        $output = Vector<Route> {};

        if ($configuration->has('routes->http')) {
            $httpRoutes = $configuration->get('routes->http');
            foreach($httpRoutes as $index => $oneHttpRoute) {

                if (!isset($oneHttpRoute['rest_api'])) {
                    throw new \Exception('The http route (#'.$index.') does not have a rest_api pointer.  This is mandatory in order to route the request properly.');
                }

                if (!isset($this->restAPIs[$oneHttpRoute['rest_api']])) {
                    throw new \Exception('The http route (#'.$index.') points to a rest_api ('.$oneHttpRoute['rest_api'].') that is not defined in the rest_apis section.');
                }

                if (!isset($oneHttpRoute['pattern'])) {
                    throw new \Exception('The http route (#'.$index.') does not have a pattern.  This is mandatory in order to route the request properly.');
                }

                if (!isset($oneHttpRoute['method'])) {
                    throw new \Exception('The http route (#'.$index.') does not have a method.  This is mandatory in order to route the request properly.');
                }

                $httpRoutePattern = $this->httpRoutePatternAdapter->convertStringToHttpRoutePattern($oneHttpRoute['pattern']);
                $httpRoute = new ConcreteHttpRoute($httpRoutePattern, $oneHttpRoute['method']);
                $output[] = new ConcreteRoute($this->restAPIs[$oneHttpRoute['rest_api']], $httpRoute);
            }
        }

        if ($configuration->has('routes->cli')) {
            $cliRoutes = $configuration->get('routes->cli');
            foreach($cliRoutes as $index => $oneCLIRoute) {

                if (!isset($oneCLIRoute['rest_api'])) {
                    throw new \Exception('The cli route (#'.$index.') does not have a rest_api pointer.  This is mandatory in order to route the request properly.');
                }

                if (!isset($this->restAPIs[$oneCLIRoute['rest_api']])) {
                    throw new \Exception('The cli route (#'.$index.') points to a rest_api ('.$oneCLIRoute['rest_api'].') that is not defined in the rest_apis section.');
                }


                $mandatoryParams = null;
                if (isset($oneCLIRoute['params']['mandatory'])) {
                    $mandatoryParams = Map<string, Variable> {};
                    foreach($oneCLIRoute['params']['mandatory'] as $keyname => $oneMandatoryParam) {
                        $mandatoryParams[$keyname] = $this->variableAdapter->convertStringToVariable($oneMandatoryParam);
                    }
                }

                $optionalParams = null;
                if (isset($oneCLIRoute['params']['optional'])) {
                    $optionalParams = Map<string, Variable> {};
                    foreach($oneCLIRoute['params']['optional'] as $keyname => $oneOptionalParam) {
                        $optionalParams[$keyname] = $this->variableAdapter->convertStringToVariable($oneOptionalParam);
                    }
                }



                $cliRoute = new ConcreteCLIRoute($mandatoryParams, $optionalParams);
                $output[] = new ConcreteRoute($this->restAPIs[$oneCLIRoute['rest_api']], null, $cliRoute);

            }
        }

        return $output;

    }

}

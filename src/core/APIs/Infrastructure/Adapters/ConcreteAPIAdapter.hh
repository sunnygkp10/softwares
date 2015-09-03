<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Functions\Repositories\FunctionCodeRepository;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Adapters;
use APIs\Domain\API;
use APIs\Domain\APIAdapter;
use APIs\Infrastructure\Adapters\ConcreteValidatorAdapter;
use APIs\Infrastructure\Adapters\ConcreteLogicAdapter;
use APIs\Infrastructure\Adapters\ConcreteRenderedParserAdapter;
use APIs\Infrastructure\Adapters\ConcreteFunctionCodeAdapter;
use APIs\Infrastructure\Adapters\ConcreteRESTAPIAdapter;
use APIs\Infrastructure\Adapters\ConcreteHttpRoutePatternAdapter;
use APIs\Infrastructure\Adapters\ConcreteRouteAdapter;
use APIs\Infrastructure\Objects\ConcreteAPI;

final class ConcreteAPIAdapter implements APIAdapter {
    private $functionCodeRepository;
    public function __construct(FunctionCodeRepository $functionCodeRepository) {
        $this->functionCodeRepository = $functionCodeRepository;
    }

    public function convertRawConfigurationToAPI(RawConfiguration $configuration): API {

        $convertToMap = function(Map<string, mixed> $data) {

            $output = Map<string, string> {};
            foreach($data as $keyname => $value) {
                $output[$keyname] = $value;
            }

            return $output;

        };

        if (!$configuration->has('name')) {
            throw new \Exception('The name field is mandatory in the raw configuration data.');
        }

        if (!$configuration->has('configuration')) {
            throw new \Exception('The configuration field is mandatory in the raw configuration data.');
        }

        if (!$configuration->has('configuration->variable_pointer')) {
            throw new \Exception('The configuration->variable_pointer field is mandatory in the raw configuration data.');
        }

        if (!$configuration->has('configuration->variable_prefix')) {
            throw new \Exception('The configuration->variable_prefix field is mandatory in the raw configuration data.');
        }

        if (!$configuration->has('configuration->variable_suffix')) {
            throw new \Exception('The configuration->variable_suffix field is mandatory in the raw configuration data.');
        }

        //retrieve some config:
        $name = $configuration->get('name');
        $variablePointer = $configuration->get('configuration->variable_pointer');
        $variablePrefix = $configuration->get('configuration->variable_prefix');
        $variableSuffix = $configuration->get('configuration->variable_suffix');

        //create some objects:
        $httpRoutePatternAdapter = new ConcreteHttpRoutePatternAdapter($variablePrefix, $variableSuffix);

        //retrieve the function codes:
        $functionCodes = $this->functionCodeRepository->retrieveMultiple($configuration);
        $functionCodeAdapter = new ConcreteFunctionCodeAdapter($functionCodes);

        //retrieve the views and parse it:
        $viewAdapter = new ConcreteViewAdapter($functionCodeAdapter);
        $views = $viewAdapter->convertRawConfigurationToViews($configuration);

        //retrieve the renderedparsers and parse it:
        $renderedParserAdapter = new ConcreteRenderedParserAdapter($functionCodeAdapter);
        $renderedParsers = $renderedParserAdapter->convertRawConfigurationToRenderedParsers($configuration);

        //retrieve the validators and parse it:
        $validatorAdapter = new ConcreteValidatorAdapter($functionCodeAdapter, $views, $renderedParsers);
        $validators = $validatorAdapter->convertRawConfigurationToValidators($configuration);

        //create some objects:
        $variableAdapter = new ConcreteVariableAdapter($variablePrefix, $variableSuffix, $variablePointer);
        $inputValidatorAdapter = new ConcreteInputValidatorAdapter($variableAdapter, $validators);
        $paramAdapter = new ConcreteParamAdapter($variableAdapter, $inputValidatorAdapter);

        //retrieve the logic and parse it:
        $logicAdapter = new ConcreteLogicAdapter($paramAdapter, $functionCodeAdapter);
        $logics = $logicAdapter->convertRawConfigurationToLogics($configuration);

        //retrieve the rest apis and parse it:
        $restAPIAdapter = new ConcreteRESTAPIAdapter($functionCodeAdapter, $paramAdapter, $logics, $views, $renderedParsers);
        $restAPIs = $restAPIAdapter->convertRawConfigurationToRESTAPIs($configuration);

        //retrieve the routes and parse it:
        $routeAdapter = new ConcreteRouteAdapter($httpRoutePatternAdapter, $variableAdapter, $restAPIs);
        $routes = $routeAdapter->convertRawConfigurationToRoutes($configuration);

        //environment variables:
        $environmentVariables = null;
        if ($configuration->has('environment_variables')) {
            $environmentVariables = $convertToMap($configuration->get('environment_variables'));
        }

        //request
        $httpRequests = null;
        if ($configuration->has('request')) {
            $httpRequests = $convertToMap($configuration->get('request'));
        }

        //cli
        $cli = null;
        if ($configuration->has('cli')) {
            $cli = $convertToMap($configuration->get('cli'));
        }

        $configurations = $convertToMap($configuration->get('configuration'));
        return new ConcreteAPI($name, $routes, $environmentVariables, $httpRequests, $cli, $configurations);
    }

}

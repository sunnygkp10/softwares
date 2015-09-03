<?hh
function handleExceptions(\Exception $exception) {

    $exceptionToData = function(\Exception $exception) use(&$exceptionToData) {

        $data = array(
            'code' => $exception->getCode(),
            'message' => $exception->getMessage(),
            'file' => $exception->getFile(),
            'line' => $exception->getLine(),
            'trace' => $exception->getTraceAsString()
        );

        $previous = $exception->getPrevious();
        if (!empty($previous)) {
            $data['previous'] = $exceptionToData($previous);
        }

        return $data;

    };

    $data = $exceptionToData($exception);
    http_response_code(500);
    echo json_encode($data);
    return true;
}

function handleError($code, $message, $file, $line) {

    $data = array(
        'code' => $code,
        'message' => $message,
        'file' => $file,
        'line' => $line
    );

    http_response_code(500);
    echo json_encode($data);
    return true;
}

function shutDown() {

}

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
set_error_handler("handleError");
register_shutdown_function("shutDown");
set_exception_handler("handleExceptions");

$autoloadFilePath = getenv('AUTOLOAD_FILEPATH');
if (empty($autoloadFilePath)) {
    throw new \Exception('The AUTOLOAD_FILEPATH must be set.');
}

include_once($autoloadFilePath);

use APIs\Domain\API;
use APIs\Infrastructure\Applications\ConcreteHttpRouterApplication;
use APIs\Infrastructure\Applications\ConcreteRESTAPIApplication;
use APIs\Infrastructure\Repositories\CurlHttpQueryRepository;
use APIs\Infrastructure\Applications\ConcreteCLIRouterApplication;

$lowerCaseKeynames = function(array $data) use(&$lowerCaseKeynames) {

    $output = Map<string, string> {};
    foreach($data as $keyname => $value) {

        $keyname = strtolower($keyname);

        if (is_array($value)) {
            $output[$keyname] = $lowerCaseKeynames($value);
            continue;
        }

        if (is_string($value)) {
            $output[$keyname] = $value;
            continue;
        }

    }

    return $output;

};

$getEnvironmentVariables = function(API $api): ?Map<string, string> {
    if (!$api->hasEnvironmentVariables()) {
        return null;
    }

    $realEnvironmentVariables = Map<string, string> {};
    $environmentVariables = $api->getEnvironmentVariables();
    foreach($environmentVariables as $keyname => $oneVariable) {
        $realEnvironmentVariables[$keyname] = getenv($oneVariable);
    }

    return $realEnvironmentVariables;
};

$getValueFromGlobalVariable = function(string $variable) {

    $matches = array();
    preg_match_all('/[\_a-zA-Z]+/s', $variable, $matches);
    if (isset($matches[0][0]) && ($matches[0][0] == $variable)) {

        if (!isset($GLOBALS[$variable])) {
            return Map<string, string> {};
        }

        return $GLOBALS[$variable];
    }

    return null;

};

$getHttpRequest = function(API $api) use($getValueFromGlobalVariable, $lowerCaseKeynames) {

    if (!$api->hasHttpRequests()) {
        return null;
    }

    $realHttpRequests = Map<string, string> {};
    $httpRequests = $api->getHttpRequests();
    foreach($httpRequests as $keyname => $value) {

        $realValue = $getValueFromGlobalVariable($value);
        if (!empty($realValue)) {
            $realHttpRequests[$keyname] = $lowerCaseKeynames($realValue);
        }

    }

    return $realHttpRequests;
};

$getCLIParams = function(API $api) use($getValueFromGlobalVariable) {

    if (!$api->hasCLI()) {
        return null;
    }

    $realCLI = Map<string, string> {};
    $cli = $api->getCLI();

    foreach($cli as $keyname => $value) {

        $realValue = $getValueFromGlobalVariable($value);
        if (!empty($realValue)) {
            $realCLI[$keyname] = $realValue;
        }

    }

    return $realCLI;

};

$mapCLIParamsInGlobals = function() use($lowerCaseKeynames) {

    $getKeyname = function(string $argument) {

        $pos = strpos($argument, '--');
        if ($pos === 0) {
            return substr($argument, $pos + 2);
        }

        $pos = strpos($argument, '-');
        if ($pos === 0) {
            return substr($argument, $pos + 1);
        }

        return null;
    };

    if (empty($GLOBALS['argv'])) {
        return;
    }

    $params = Map<string, mixed> {};
    $lastKeyname = null;
    foreach($GLOBALS['argv'] as $oneArgument) {

        $keyname = $getKeyname($oneArgument);

        if (empty($keyname)) {
            if (!empty($lastKeyname)) {
                $params[$lastKeyname] = $oneArgument;
            }

            continue;
        }

        $params[$keyname] = null;
        $lastKeyname = $keyname;
    }

    $GLOBALS['argv'] = $params;

};

$getAPI = function(string $softwareName = null) {

    if (empty($softwareName)) {
        throw new \Exception('The softwareName has not been provided.');
    }

    $apiUrl = getenv('API_URL');
    $params = array(
        'name' => $softwareName
    );
    $resourceUrl = $apiUrl.'?'.http_build_query($params);

    $json = @file_get_contents($resourceUrl);
    $data = @json_decode($json, true);

    if (!isset($data['api'])) {
        throw new \Exception('The software ('.$softwareName.') does not exists.');
    }

    $serialize = base64_decode($data['api']);
    return unserialize($serialize);
};

$routeCLI = function() use($mapCLIParamsInGlobals, $getAPI, $getEnvironmentVariables, $getCLIParams) {

    $mapCLIParamsInGlobals();
    $softwareName = !empty($GLOBALS['argv']['software_name']) ? $GLOBALS['argv']['software_name'] : null;

    $api = $getAPI($softwareName);
    $environmentVariables = $getEnvironmentVariables($api);

    $httpQueryRepository = new CurlHttpQueryRepository();
    $restAPIApplication = new ConcreteRESTAPIApplication($httpQueryRepository, $environmentVariables);

    $router = new ConcreteCLIRouterApplication($api, $restAPIApplication);
    $router->route();
};

$routeHttp = function() use($getAPI, $getEnvironmentVariables, $getHttpRequest)  {

    $host = !empty($_SERVER['HTTP_HOST']) ? $_SERVER['HTTP_HOST'] : '';
    $exploded = explode('.', $host);
    $softwareName = array_shift($exploded);

    $api = $getAPI($softwareName);
    $environmentVariables = $getEnvironmentVariables($api);
    $httpRequest = $getHttpRequest($api);

    $httpQueryRepository = new CurlHttpQueryRepository();
    $restAPIApplication = new ConcreteRESTAPIApplication($httpQueryRepository, $environmentVariables, $httpRequest);

    $router = new ConcreteHttpRouterApplication($api, $restAPIApplication);
    $router->route($_SERVER['REQUEST_URI'], $_SERVER['REQUEST_METHOD']);
};

$route = function() use($routeCLI, $routeHttp) {

    $routes = array(
        'cli' => $routeCLI,
        'http' => $routeHttp
    );

    $name = php_sapi_name();
    if (isset($routes[$name])) {
        return $routes[$name]();
    }

    return $routes['http']();
};

$route();

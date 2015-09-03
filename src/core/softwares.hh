<?hh
//curl --data "config_url=http%3A%2F%2Fcode.irestful.com%2Fsoftwares%2Fdatastore%2Fapis%2Fdatastore.json" http://softwares.irestful.com
//http://softwares.irestful.com:8080/?name=datastore-apis
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

$convertToMap = function(array $data) use(&$convertToMap) {

    $output = Map<string, mixed> {};
    foreach($data as $keyname => $oneData) {

        if (is_array($oneData)) {
            $oneData = $convertToMap($oneData);
        }

        $output[$keyname] = $oneData;
    }

    return $output;

};

$autoloadFilePath = getenv('AUTOLOAD_FILEPATH');
if (empty($autoloadFilePath)) {
    throw new \Exception('The AUTOLOAD_FILEPATH must be set.');
}
include_once($autoloadFilePath);

use APIs\Infrastructure\Repositories\ConcreteRawConfigurationRepository;
use APIs\Infrastructure\Adapters\ConcreteAPIAdapter;
use APIs\Infrastructure\Applications\APIApplication;
use APIs\Infrastructure\Repositories\CurlHttpQueryRepository;
use APIs\Infrastructure\Adapters\ConcreteURLAdapter;
use APIs\Infrastructure\Repositories\ConcreteFunctionCodeRepository;
use APIs\Infrastructure\Services\FileAPIService;

$variableDelimiter = '->';
$databasePath = '../database';

$urlAdapter = new ConcreteURLAdapter();

$configurationRepository = new ConcreteRawConfigurationRepository($variableDelimiter);
$httpQueryRepository = new CurlHttpQueryRepository();
$functionCodeRepository = new ConcreteFunctionCodeRepository($httpQueryRepository, $urlAdapter);

$apiAdapter = new ConcreteAPIAdapter($functionCodeRepository);

$apiService = new FileAPIService($databasePath);

$query = $convertToMap($_GET);
$request = $convertToMap($_POST);
$application = new APIApplication($apiService, $configurationRepository, $apiAdapter, $query, $request, $databasePath);
$application->execute();

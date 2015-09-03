<?hh
namespace APIs\Infrastructure\Applications;
use APIs\Applications\Application;
use APIs\Domain\APIAdapter;
use APIs\Domain\Configurations\Repositories\RawConfigurationRepository;
use APIs\Infrastructure\Objects\Representations\ConcreteAPIRepresentation;
use APIs\Domain\Services\APIService;

final class APIApplication implements Application {
    private $apiService;
    private $configurationRepository;
    private $apiAdapter;
    private $query;
    private $request;
    private $databasePath;
    public function __construct(APIService $apiService, RawConfigurationRepository $configurationRepository, APIAdapter $apiAdapter, Map<string, mixed> $query, Map<string, mixed> $request, string $databasePath) {
        $this->apiService = $apiService;
        $this->configurationRepository = $configurationRepository;
        $this->apiAdapter = $apiAdapter;
        $this->query = $query;
        $this->request = $request;
        $this->databasePath = $databasePath;
    }

    public function execute() {

        if (isset($this->request['config_url'])) {
            $url = $this->request['config_url'];
            $config = $this->configurationRepository->retrieve($url);
            $api = $this->apiAdapter->convertRawConfigurationToAPI($config);

            $original = $config->getOriginal();
            $representation = new ConcreteAPIRepresentation($url, $original, $api);
            $this->apiService->save($representation);
            return;
        }

        if (isset($this->query['name'])) {

            $name = $this->query['name'];
            $folders = str_split($name, 2);
            $path = $this->databasePath.'/'.implode('/', $folders);
            $filePath = $path.'/'.$name.'.json';

            if (!file_exists($filePath)) {
                throw new \Exception('The requested api ('.$name.') does not exists.', 404);
            }

            http_response_code(200);
            header('Content-Type: application/json');
            echo file_get_contents($filePath);
            return;
        }

        throw new \Exception('The requested route does not exists.', 404);
    }
}

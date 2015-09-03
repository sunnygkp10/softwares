<?hh
namespace APIs\Infrastructure\Services;
use APIs\Domain\Services\APIService;
use APIs\Domain\Services\APIRepresentation;

final class FileAPIService implements APIService {
    private $saveToDirectoryPath;
    public function __construct(string $saveToDirectoryPath) {

        if (!file_exists($saveToDirectoryPath)) {
            throw new \Exception('The given directory ('.$saveToDirectoryPath.') does not exists.  Therefore, its impossible to save API objects to it.');
        }

        $this->saveToDirectoryPath = $saveToDirectoryPath;
    }

    public function save(APIRepresentation $representation): void {

        $api = $representation->getAPI();
        $serialized = serialize($api);
        $encoded = base64_encode($serialized);

        $data = array(
            'url' => $representation->getUrl(),
            'original' => $representation->getOriginal(),
            'api' => $encoded
        );

        $json = json_encode($data);

        $name = $api->getName();
        $folders = str_split($name, 2);
        $path = $this->saveToDirectoryPath.'/'.implode('/', $folders);
        if (!file_exists($path)) {
            mkdir($path, 0777, true);
        }

        $filePath = $path.'/'.$name.'.json';
        file_put_contents($filePath, $json);

    }

    private function createFolders(string $name) {
        $split = str_split($name, 2);
        return implode('/', $split).'/'.$name.'.json';
    }

}

<?hh
namespace APIs\Infrastructure\Repositories;
use APIs\Domain\Configurations\Repositories\RawConfigurationRepository;
use APIs\Infrastructure\Objects\ConcreteRawConfiguration;

final class ConcreteRawConfigurationRepository implements RawConfigurationRepository {
    private $variableDelimiter;
    public function __construct(string $variableDelimiter) {
        $this->variableDelimiter = $variableDelimiter;
    }

    public function retrieve(string $url) {

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

        $data = $this->retrieveMap($url);
        $map = $convertToMap($data['data']);
        return new ConcreteRawConfiguration($data['content'], $map, $this->variableDelimiter);

    }

    private function retrieveMaps(Vector<string> $urls) {
        $output = Vector<string> {};
        foreach($urls as $oneUrl) {
            $map = $this->retrieveMap($oneUrl);
            $output[] = $map['data'];
        }

        return $output;
    }

    private function retrieveMap(string $url) {

        $convertToVector = function(array $data) {

            $output = Vector<string> {};
            foreach($data as $oneElement) {
                $output[] = $oneElement;
            }

            return $output;

        };

        $replaceInData = function(array $data, array $replacement) use(&$replaceInData) {

            foreach($replacement as $keyname => $oneReplacement) {
                if (isset($data[$keyname]) && is_array($oneReplacement)) {
                    $oneReplacement = $replaceInData($data[$keyname], $oneReplacement);
                }

                $data[$keyname] = $oneReplacement;
            }

            return $data;

        };

        $content = @file_get_contents($url);
        $data = @json_decode($content, true);
        if (empty($data)) {
            throw new \Exception('The content retrieved from the url ('.$url.') is not valid json configuration.');
        }

        if (isset($data['parents'])) {
            $parentUrls = $convertToVector($data['parents']);
            $parents = $this->retrieveMaps($parentUrls);
            foreach($parents as $oneParentData) {
                $data = $replaceInData($data, $oneParentData);
            }

            unset($data['parents']);
        }

        return Map<string, mixed>{
            'data' => $data,
            'content' => $content
        };
    }

}

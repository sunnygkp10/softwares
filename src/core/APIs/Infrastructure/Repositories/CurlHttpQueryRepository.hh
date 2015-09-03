<?hh
namespace APIs\Infrastructure\Repositories;
use APIs\Domain\Https\Repositories\HttpQueryRepository;
use APIs\Domain\Https\Repositories\HttpQueryRetrieverCriteria;
use APIs\Domain\Https\URLs\URL;
use APIs\Infrastructure\Objects\ConcreteHttpQuery;
use APIs\Domain\Https\HttpQuery;

final class CurlHttpQueryRepository implements HttpQueryRepository {

    public function __construct() {

    }

    public function retrieve(Map<string, HttpQueryRetrieverCriteria> $criterias): Map<string, HttpQuery> {

        $resources = Map<string, mixed> {};
        $multiResource = curl_multi_init();
        foreach($criterias as $keyname => $oneCriteria) {

            $url = $oneCriteria->getUrl()->get();
            $port = $oneCriteria->hasPort() ? $oneCriteria->getPort(): null;
            $data = $oneCriteria->hasData() ? $oneCriteria->getData(): null;

            $resources[$keyname] = $this->createResource($url, $oneCriteria->getHttpMethod(), $port, $data);
            curl_multi_add_handle($multiResource, $resources[$keyname]);
        }

        //process the multi requests:
        $this->processMultiRequests($multiResource);

        //we fetch the content:
        $output = Map<string, HttpQuery> {};
        foreach($resources as $keyname => $resource) {
            $httpCode = curl_getinfo($resource, CURLINFO_HTTP_CODE);
            $content = curl_multi_getcontent($resource);
            if (empty($content)) {
                $content = '';
            }

            $url = $criterias[$keyname]->getUrl();
            $output[$keyname] = new ConcreteHttpQuery($url, $content, $httpCode);
        }

        return $output;

    }

    private function createResource(string $url, string $httpMethod, integer $port = null, Map<string, string> $data = null) {

        $curlParams = array();
        $curlParams[CURLOPT_CUSTOMREQUEST] = strtoupper($httpMethod);
        if (!empty($data)) {
            $curlParams[CURLOPT_POSTFIELDS] = http_build_query($data);
        }

        $curlParams[CURLOPT_URL] = $url;
        $curlParams[CURLOPT_RETURNTRANSFER] = true;
        $curlParams[CURLOPT_FOLLOWLOCATION] = true;

        if (!empty($port)) {
            $curlParams[CURLOPT_PORT] = $port;
        }

        $resource = curl_init();
        curl_setopt_array($resource, $curlParams);
        return $resource;
    }

    private function processMultiRequests($resource) {

        $active = null;
        do {
            $output = curl_multi_exec($resource, $active);
        } while ($output == CURLM_CALL_MULTI_PERFORM);

        while ($active && $output == CURLM_OK) {
            if (curl_multi_select($resource) == -1) {
                continue;
            }

            do {
                $output = curl_multi_exec($resource, $active);
            } while ($output == CURLM_CALL_MULTI_PERFORM);
        }
    }

}

<?hh
function execute(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    $createResource = function(string $url, string $httpMethod, integer $port = null, Map<string, string> $data = null) {

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
    };

    $processMultiRequests = function($resource) {

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
    };

    $resources = Map<string, mixed> {};
    $multiResource = curl_multi_init();
    foreach($params['queries'] as $keyname => $oneQuery) {

        if (!isset($oneQuery['host'])) {
            throw new \Exception('The queries[i]->host param is mandatory in order to execute the curl request.', 500);
        }

        if (!isset($oneQuery['http_method'])) {
            throw new \Exception('The queries[i]->http_method param is mandatory in order to execute the curl request.', 500);
        }

        $port = isset($oneQuery['port']) ? $oneQuery['port'] : null;
        $data = isset($oneQuery['data']) ? $oneQuery['data'] : null;

        $resources[$keyname] = $createResource($oneQuery['host'], $oneQuery['http_method'], $port, $data);
        curl_multi_add_handle($multiResource, $resources[$keyname]);
    }

    //process the multi requests:
    $processMultiRequests($multiResource);

    //we fetch the content:
    $output = Map<string, HttpQuery> {};
    foreach($resources as $keyname => $resource) {
        $httpCode = curl_getinfo($resource, CURLINFO_HTTP_CODE);
        $content = curl_multi_getcontent($resource);
        if (empty($content)) {
            $content = '';
        }

        $output[$keyname] = Map<string, string> {
            'http_code' => $httpCode,
            'content' => $content
        };
    }

    return $output;

};

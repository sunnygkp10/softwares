<?hh
function execute(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($params['host'])) {
        throw new \Exception('The host param is mandatory in order to execute the curl request.', 500);
    }

    if (!isset($params['uri'])) {
        throw new \Exception('The uri param is mandatory in order to execute the curl request.', 500);
    }

    if (!isset($params['http_method'])) {
        throw new \Exception('The http_method param is mandatory in order to execute the curl request.', 500);
    }

    if (!isset($params['port'])) {
        throw new \Exception('The port param is mandatory in order to execute the curl request.', 500);
    }

    try {

        $url = $params['host'].$params['uri'];

        $curlParams = array();
        $curlParams[CURLOPT_CUSTOMREQUEST] = strtoupper($params['http_method']);

        if (isset($params['data']) && !empty($params['data'])) {
            $curlParams[CURLOPT_POSTFIELDS] = http_build_query($params['data']);
        }

        $curlParams[CURLOPT_URL] = $url;
        $curlParams[CURLOPT_RETURNTRANSFER] = true;
        $curlParams[CURLOPT_FOLLOWLOCATION] = true;
        $curlParams[CURLOPT_PORT] = $params['port'];

        $resource = curl_init();
        curl_setopt_array($resource, $curlParams);

        $content = curl_exec($resource);
        $httpCode = curl_getinfo($resource, CURLINFO_HTTP_CODE);
        $error = curl_error($resource);
        $contentType = curl_getinfo($resource, CURLINFO_CONTENT_TYPE);

        $output = array(
            'http_code' => $httpCode,
            'content' => $content,
            'content_type' => $contentType,
            'error' => $error
        );
        curl_close($resource);

        return $output;

    } catch (\Exception $exception) {
        throw new \Exception('There was a problem while executing a curl request: '.$exception->getMessage(), 500);
    }

};

<?hh

function validator(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($subLogics['curl_execute'])) {
        throw new \Exception('The curl_execute subLogic is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['validator']['language']['host'])) {
        throw new \Exception('The validator->language->host param is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['validator']['language']['port'])) {
        throw new \Exception('The validator->language->port param is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['validator']['function']['name'])) {
        throw new \Exception('The validator->function->name param is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['validator']['function']['url'])) {
        throw new \Exception('The validator->function->url param is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['value'])) {
        throw new \Exception('The value param is mandatory in order to execute the validator.', 500);
    }


    $data = array(
        'type' => 'input_validator',
        'name' => $params['validator']['function']['name'],
        'code' => file_get_contents($params['validator']['function']['url']),
        'input' => $params['value']
    );

    $curlParams = Map<string, string> {
        'host' => $params['validator']['language']['host'],
        'port' => $params['validator']['language']['port'],
        'data' => $data,
        'uri' => '/',
        'http_method' => 'post'
    };

    $content = $subLogics['curl_execute']($curlParams);
    if ($content['http_code'] != 200) {
        throw new \Exception('There was an exception while executing the validator.  The exception was: '.$content['error'], $content['http_code']);
    }

    return $content['content'];

};

<?hh

function validator(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {



    if (!isset($subLogics['curl_execute'])) {
        throw new \Exception('The curl_execute subLogic is mandatory in order to execute the validator.', 500);
    }

    if (!isset($params['validators'])) {
        throw new \Exception('The validators param is mandatory in order to execute the validator.', 500);
    }

    $queries = Map<string, Map<string, mixed>> {};
    foreach($params['validators'] as $keyname => $oneValidator) {

        if (!isset($oneValidator['validator']['language']['host'])) {
            throw new \Exception('The validators[i]->validator->language->host param is mandatory in order to execute the validator.', 500);
        }

        if (!isset($oneValidator['validator']['language']['port'])) {
            throw new \Exception('The validators[i]->validator->language->port param is mandatory in order to execute the validator.', 500);
        }

        if (!isset($oneValidator['validator']['function']['name'])) {
            throw new \Exception('The validators[i]->validator->function->name param is mandatory in order to execute the validator.', 500);
        }

        if (!isset($oneValidator['validator']['function']['url'])) {
            throw new \Exception('The validators[i]->validator->function->url param is mandatory in order to execute the validator.', 500);
        }

        if (!isset($oneValidator['value'])) {
            throw new \Exception('The validators[i]->value param is mandatory in order to execute the validator.', 500);
        }

        $code = @file_get_contents($oneValidator['validator']['function']['url']);
        $queries[$keyname] = Map<string, mixed> {
            'port' => (integer) $oneValidator['validator']['language']['port'],
            'http_method' => 'post',
            'host' => $oneValidator['validator']['language']['host'],
            'data' => Map<string, string> {
                'type' => 'input_validator',
                'name' => $oneValidator['validator']['function']['name'],
                'code' => $code,
                'input' => $oneValidator['value']
            }
        };

    }

    $curlParams = Map<string, mixed> {
        'queries' => $queries
    };

    return $subLogics['curl_execute']($curlParams);

};

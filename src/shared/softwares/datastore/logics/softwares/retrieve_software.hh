<?hh

function retrieveSoftware(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($params['host'])) {
        throw new \Exception('The host param is mandatory in order to retrieve a software.');
    }

    if (!isset($params['port'])) {
        throw new \Exception('The port param is mandatory in order to retrieve a software.');
    }

    if (!isset($params['software_name'])) {
        throw new \Exception('The software_name param is mandatory in order to retrieve a software.');
    }

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute subLogic is mandatory in order to retrieve a software.');
    }

    $params['uri'] = '/'.$params['software_name'];
    $params['http_method'] = 'get';
    unset($params['software_name']);
    
    return $subLogics['execute']($params);

}

<?hh

function insertSoftware(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($params['host'])) {
        throw new \Exception('The host param is mandatory in order to insert a software.');
    }

    if (!isset($params['port'])) {
        throw new \Exception('The port param is mandatory in order to insert a software.');
    }

    if (!isset($params['data'])) {
        throw new \Exception('The data param is mandatory in order to insert a software.');
    }

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute subLogic is mandatory in order to insert a software.');
    }

    $params['uri'] = '/';
    $params['http_method'] = 'post';
    return $subLogics['execute']($params);

}

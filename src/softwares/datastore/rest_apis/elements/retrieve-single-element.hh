<?hh
function retrieveSingleElement(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['retrieve_one'])) {
        throw new \Exception('The retrieve_one subLogic is mandatory in order to retrieve a single element.', 500);
    }

    if (!isset($params['uuid'])) {
        throw new \Exception('The uuid param is mandatory in order to retrieve a single element.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to retrieve a single element.', 500);
    }

    $query = 'select * from '.$params['container_name'].' where uuid = :uuid;';
    $queryParams = array(
        ':uuid' => hex2bin(str_replace('-', '', $params['uuid']))
    );

    $retrieveParams = Map<string, string> {
        'query' => $query,
        'params' => $queryParams
    };

    $data = $subLogics['retrieve_one']($retrieveParams);

    if (!isset($data['uuid'])) {
        throw new \Exception('The requested element (uuid: '.$params['uuid'].') could not be found.', 404);
    }

    return $data;

};

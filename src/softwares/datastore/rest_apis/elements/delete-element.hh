<?hh
function deleteElement(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($params['uuid'])) {
        throw new \Exception('The uuid param is mandatory in order to delete an element.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to delete an element.', 500);
    }

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute closure is mandatory in order to delete an element.', 500);
    }

    $uuid = hex2bin(str_replace('-', '', $params['uuid']));
    $query = 'delete from '.$params['container_name'].' where uuid = :uuid;';
    $queryParams = Map<string, string> {
        ':uuid' => $uuid
    };

    $executeParams = Map<string, string> {
        'query' => $query,
        'params' => $queryParams
    };

    $subLogics['execute']($executeParams);
};

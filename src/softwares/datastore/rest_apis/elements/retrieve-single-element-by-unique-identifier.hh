<?hh
function retrieveSingleElement(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['retrieve_one'])) {
        throw new \Exception('The retrieve_one subLogic is mandatory in order to retrieve a single element.', 500);
    }

    if (!isset($params['field_name'])) {
        throw new \Exception('The field_name param is mandatory in order to retrieve a single element.', 500);
    }

    if (!isset($params['field_value'])) {
        throw new \Exception('The field_value param is mandatory in order to retrieve a single element.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to retrieve a single element.', 500);
    }


    $query = 'select * from '.$params['container_name'].' where '.$params['field_name'].' = :field_value;';
    $queryParams = array(
        ':field_value' => $params['field_value']
    );

    $retrieveParams = Map<string, string> {
        'query' => $query,
        'params' => $queryParams
    };

    $data = $subLogics['retrieve_one']($retrieveParams);
    if (!isset($data['uuid'])) {
        throw new \Exception('The requested element ('.$params['field_name'].': '.$params['field_value'].') could not be found.', 404);
    }

    return $data;

};

<?hh
function retrieveTotalAmountOfElements(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['retrieve_one'])) {
        throw new \Exception('The retrieve_one subLogic is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to retrieve the total amount of elements.');
    }

    if (empty($params['index'])) {
        $params['index'] = 0;
    }

    $query = 'select count(*) as amount from '.$params['container_name'].';';
    $retrieveParams = Map<string, string> {
        'query' => $query
    };

    $data = $subLogics['retrieve_one']($retrieveParams);
    return $data['amount'];

};

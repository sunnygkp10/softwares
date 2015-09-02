<?hh
function retrieveElementsWithPagination(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['retrieve'])) {
        throw new \Exception('The retrieve subLogic is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($subLogics['retrieve_total_amount_of_elements'])) {
        throw new \Exception('The retrieve_total_amount_of_elements subLogic is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($params['index'])) {
        throw new \Exception('The index param is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($params['amount'])) {
        throw new \Exception('The amount param is mandatory in order to retrieve elements with pagination.');
    }

    if (!isset($params['order_by'])) {
        throw new \Exception('The order_by param is mandatory in order to retrieve elements with pagination.');
    }

    if (empty($params['index'])) {
        $params['index'] = 0;
    }

    $query = 'select * from '.$params['container_name'].' order by '.$params['order_by'].' limit '.$params['index'].','.$params['amount'].';';
    $retrieveParams = Map<string, string> {
        'query' => $query
    };

    $totalAmount = $subLogics['retrieve_total_amount_of_elements']();
    $data = $subLogics['retrieve']($retrieveParams);

    return Map<string, mixed> {
        'index' => $params['index'],
        'amount' => $params['amount'],
        'total_amount' => $totalAmount,
        'remaining_amount' => $totalAmount - ($params['index'] + $params['amount']),
        'elements' => $data
    };
};

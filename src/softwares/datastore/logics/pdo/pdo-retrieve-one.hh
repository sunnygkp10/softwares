<?hh

function PDORetrieveOne(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($subLogics['retrieve'])) {
        throw new \Exception('The retrieve closure is mandatory in order to retrieve one row of data from the database (PDORetrieveOne).', 500);
    }

    if (!isset($params['query'])) {
        throw new \Exception('The query param is mandatory in order to retrieve one row of data from the database (PDORetrieveOne).', 500);
    }

    try {

        $queryParams = Map<string, string> {'query' => $params['query']};
        if (isset($params['params'])) {
            $queryParams['params'] = $params['params'];
        }

        $data = $subLogics['retrieve']($queryParams);
        if (empty($data)) {
            return array();
        }

        $keys = array_keys($data);
        return $data[$keys[0]];

    } catch (\PDOException $exception) {
        throw new \Exception($exception->getMessage(), 500);
    }

};

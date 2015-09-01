<?hh

function PDOExecute(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    if (!isset($subLogics['connector'])) {
        throw new \Exception('The connector closure is mandatory in order to retrieve one row of data from the database (PDOExecute).', 500);
    }

    if (!isset($params['query'])) {
        throw new \Exception('The query param is mandatory in order to retrieve one row of data from the database (PDOExecute).', 500);
    }

    try {


        $pdo = $subLogics['connector']();
        
        $statement = $pdo->prepare($params['query']);
        (isset($params['params']) ? $statement->execute($params['params']) : $statement->execute());

    } catch (\PDOException $exception) {
        throw new \Exception($exception->getMessage(), 500);
    }

};

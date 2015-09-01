<?hh

function PDORetrieve(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    $convertToMap = function(array $data) {

        $convertRowToMap = function(array $data) {

            $output = Map<string, string> {};
            foreach($data as $keyname => $oneElement) {
                $output[$keyname] = $oneElement;
            }

            return $output;

        };

        $output = Vector<Map<string, string>> {};
        foreach($data as $index => $oneData) {
            $output[] = $convertRowToMap($oneData);
        }

        return $output;

    };

    if (!isset($subLogics['connector'])) {
        throw new \Exception('The connector closure is mandatory in order to retrieve from a database (PDORetrieve).', 500);
    }

    if (!isset($params['query'])) {
        throw new \Exception('The query param is mandatory in order to retrieve from a database (PDORetrieve).', 500);
    }

    try {

        $pdo = $subLogics['connector']();
        $statement = $pdo->prepare($params['query']);
        (isset($params['params']) ? $statement->execute($params['params']) : $statement->execute());
        $data = $statement->fetchAll(\PDO::FETCH_ASSOC);
        return $convertToMap($data);

    } catch (\PDOException $exception) {
        throw new \Exception($exception->getMessage(), 500);
    }

};

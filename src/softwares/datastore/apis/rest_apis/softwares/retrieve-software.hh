<?hh

function retrieveSoftware(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($params['software_name'])) {
        throw new \Exception('The software_name is mandatory in order to retrieve a software.', 500);
    }

    if (!isset($subLogics['connector'])) {
        throw new \Exception('The connector closure is mandatory in order to retrieve a software.', 500);
    }

    $filterTables = function(array $tables): Vector<string> {
        $containers = Vector<string> {};
        foreach($tables as $oneRow) {

            if (isset($oneRow[0])) {
                $containers[] = $oneRow[0];
            }

        }

        sort($containers);
        return $containers;
    };

    try {

        //retrieve connector:
        $pdo = $subLogics['connector']();

        //retrieve all the tables:
        $query = "show tables;";
        $statement = $pdo->prepare($query);
        $statement->execute();
        $tables = $statement->fetchAll();

        $data = Map<string, mixed> {
            'name' => $params['software_name'],
            'containers' => $filterTables($tables)
        };

        return $data;

    } catch (\Exception $exception) {
        throw new \Exception($exception->getMessage(), 404);
    }

};

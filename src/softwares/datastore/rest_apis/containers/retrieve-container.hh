<?hh

function retrieveContainer(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    $parseType = function(string $type) {

        $matches = array();
        preg_match_all('/([a-z]+)\(([0-9]+)\)/s', $type, $matches);

        if (isset($matches[1][0]) && isset($matches[2][0])) {
            return array(
                'type' => $matches[1][0],
                'length' => $matches[2][0]
            );
        }

        return array(
            'type' => $type
        );
    };

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to retrieve a container.', 500);
    }

    if (!isset($subLogics['retrieve'])) {
        throw new \Exception('The retrieve closure is mandatory in order to retrieve a container.', 500);
    }

    //we retrieve the fields related to the table:
    $query = 'show full columns from '.$params['container_name'];

    $queryParams = Map<string, string> {
        'query' => $query
    };
    $describe = $subLogics['retrieve']($queryParams);

    $fields = array();
    foreach($describe as $oneRow) {


        $fieldName = $oneRow['Field'];
        $fields[$fieldName] = $parseType($oneRow['Type']);

        if ($oneRow['Null'] == 'YES') {
            $fields[$fieldName]['default'] = 'null';
        }

        if ($oneRow['Key'] == 'PRI') {
            $fields[$fieldName]['is_primary_key'] = true;
        }

        if ($oneRow['Key'] == 'UNI') {
            $fields[$fieldName]['is_unique_key'] = true;
        }

        if (!empty($oneRow['Default'])) {
            $fields[$fieldName]['default'] = $oneRow['Default'];
        }

        if (!empty($oneRow['Comment'])) {

            $decoded = json_decode($oneRow['Comment'], true);
            if (isset($decoded['validator'])) {
                $fields[$fieldName]['validator'] = $decoded['validator'];
            }
        }

    }

    $data = Map<string, mixed> {
        'name' => $params['container_name'],
        'fields' => $fields
    };

    return $data;

};

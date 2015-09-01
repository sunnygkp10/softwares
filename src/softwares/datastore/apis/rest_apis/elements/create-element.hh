<?hh
function createElement(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute subLogic is mandatory in order to create an element.', 500);
    }

    if (!isset($subLogics['validator'])) {
        throw new \Exception('The validator subLogic is mandatory in order to create an element.', 500);
    }

    if (!isset($subAPIs['retrieve_container'])) {
        throw new \Exception('The retrieve_container subAPI is mandatory in order to create an element.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to create an element.', 500);
    }

    if (!isset($params['data'])) {
        throw new \Exception('The data param is mandatory in order to create an element.', 500);
    }

    $input = json_decode($params['data'], true);
    $container = $subAPIs['retrieve_container']();

    foreach($input as $name => $value) {

        if (!isset($container['fields'][$name])) {
            //throw
        }

        $field = $container['fields'][$name];
        if (!isset($field['validator'])) {
            continue;
        }

        //execute the validator:
        $validatorParams = Map<string, string> {
            'validator' => $field['validator'],
            'value' => $value
        };

        $output = $subLogics['validator']($validatorParams);
        if ($output != $value) {
            throw new \Exception("The field's value cannot be validated against its validator.  Field name: ".$name.", field value: ".$value, 500);
        }

    }

    $input['uuid'] = hex2bin(str_replace('-', '', $input['uuid']));

    $fields = array_keys($input);
    $variables = array();
    $map = array();
    foreach($input as $field => $value) {

        $variable = ':'.$field;
        $map[$variable] = $value;
        $variables[] = $variable;

    }

    $query = 'insert into '.$params['container_name'].' ('.implode(', ', $fields).') values('.implode(', ', $variables).');';
    $queryParams = Map<string, string> {
        'query' => $query,
        'params' => $map
    };

    $subLogics['execute']($queryParams);
};

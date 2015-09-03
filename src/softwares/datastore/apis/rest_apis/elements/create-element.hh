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

    $input = $params['data'];
    $container = $subAPIs['retrieve_container']();
    $validators = Map<string, Map<string, string>> {};
    foreach($input as $name => $value) {

        if (!isset($container['fields'][$name])) {
            //throw
        }

        $field = $container['fields'][$name];
        if (!isset($field['validator'])) {
            continue;
        }

        //execute the validator:
        $validators[$name] = Map<string, string> {
            'validator' => $field['validator'],
            'value' => $value
        };
    }

    $validatorParams = Map<string, Map<string, Map<string, string>>> {
        'validators' => $validators
    };
    $output = $subLogics['validator']($validatorParams);

    foreach($output as $keyname => $oneOutput) {

        if ($oneOutput['http_code'] != 200) {
            throw new \Exception('The field ('.$keyname.') was not able to validate.  The returned http_code was: '.$oneOutput['http_code'].'.  The content was: '.$oneOutput['content']);
        }

        if ($oneOutput['content'] != $validators[$keyname]['value']) {
            throw new \Exception('The field () did not validate.  The input was: '.$validators[$keyname]['value'].'.  The output was: '.$oneOutput['content']);
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

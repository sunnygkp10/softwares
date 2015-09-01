<?hh
function elementFields(array $params) {
    
    if (!isset($params['uuid'])) {
        throw new \Exception('The uuid field is mandatory.', 500);
    }

    if (!isset($params['created_on'])) {
        throw new \Exception('The created_on field is mandatory.', 500);
    }

    if (isset($params['last_updated_on'])) {
        throw new \Exception('The last_updated_on field cannot be set when inserting a new element.', 500);
    }

    return json_encode($params);
};

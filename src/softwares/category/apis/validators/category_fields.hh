<?hh
function categoryFields(Map<string, mixed> $params) {

    if (!isset($params['uuid'])) {
        throw new \Exception('The uuid field is mandatory.', 500);
    }

    if (!isset($params['slug'])) {
        throw new \Exception('The slug field is mandatory.', 500);
    }

    if (!isset($params['link_anchor'])) {
        throw new \Exception('The link_anchor field is mandatory.', 500);
    }

    if (!isset($params['title'])) {
        throw new \Exception('The title field is mandatory.', 500);
    }

    if (!isset($params['description'])) {
        $params['description'] = '';
    }

    if (!isset($params['created_on'])) {
        throw new \Exception('The created_on field is mandatory.', 500);
    }

    if (isset($params['last_updated_on'])) {
        throw new \Exception('The last_updated_on field cannot be set when inserting a new element.', 500);
    }

    return $params;
};

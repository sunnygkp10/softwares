<?hh
function singleElement(Map<string, string> $params, Map<string, \Closure> $subViews = null) {

    $convertBinaryToUuid = function(string $binary) {

        $uuid = strtolower(bin2hex($binary));

        $matches = array();
        preg_match_all('/([a-z0-9]{8})([a-z0-9]{4})([a-z0-9]{4})([a-z0-9]{4})([a-z0-9]{12})/s', $uuid, $matches);

        if ($matches[0][0] != $uuid) {
            throw new \Exception('The given uuid is invalid.  It must be a binary representation of a uuid without dashes.', 500);
        }

        $uuidWithDashes = array();
        unset($matches[0]);
        foreach($matches as $oneData) {
            $uuidWithDashes[] = $oneData[0];
        }

        return implode('-', $uuidWithDashes);
    };

    if (!isset($params['uuid'])) {
        throw new \Exception('The uuid index, in the data, is mandatory in order to render the singleElement view properly.', 500);
    }

    //convert the binary uuid to a human readable uuid:
    $params['uuid'] = $convertBinaryToUuid($params['uuid']);

    //remove the last_updated_on if its null:
    if (!isset($params['last_updated_on']) || is_null($params['last_updated_on'])) {
        unset($params['last_updated_on']);
    }

    return $params;

};

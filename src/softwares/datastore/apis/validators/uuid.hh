<?hh
function uuid(string $uuid) {
    
    $matches = array();
    preg_match_all('/[a-zA-Z0-9]{8}\-[a-zA-Z0-9]{4}\-[a-zA-Z0-9]{4}\-[a-zA-Z0-9]{4}\-[a-zA-Z0-9]{12}/s', $uuid, $matches);

    if (isset($matches[0][0]) && ($matches[0][0] == $uuid)) {
        return $matches[0][0];
    }

    throw new \Exception('The given input ('.$uuid.') is not a valid uuid.', 500);
};

<?hh
function slug(string $slug) {

    $matches = array();
    preg_match_all('/[a-zA-Z0-9\-]+/s', $slug, $matches);

    if (isset($matches[0][0]) && ($matches[0][0] == $slug)) {
        return strtolower($matches[0][0]);
    }

    throw new \Exception('The given input ('.$slug.') is not a valid uuid.', 500);
};

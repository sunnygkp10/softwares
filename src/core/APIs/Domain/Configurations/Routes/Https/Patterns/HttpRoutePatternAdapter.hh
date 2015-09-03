<?hh
namespace APIs\Domain\Configurations\Routes\Https\Patterns;

interface HttpRoutePatternAdapter {
    public function convertStringToHttpRoutePattern(string $pattern): HttpRoutePattern;
}

<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Routes\Https\HttpRoute;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern;

final class ConcreteHttpRoute implements HttpRoute {
    private $pattern;
    private $method;
    private static $possibleHttpMethods = Vector<string> {
        'get', 'post', 'put', 'patch', 'delete'
    };
    public function __construct(HttpRoutePattern $pattern, string $method) {

        $method = strtolower($method);
        if (!in_array($method, self::$possibleHttpMethods)) {
            throw new \Exception('The given http method ('.$method.') is not in the approved list.  The approved list is: '.implode(', ', self::$possibleHttpMethods));
        }

        $this->pattern = $pattern;
        $this->method = $method;

    }

    public function getPattern(): HttpRoutePattern {
        return $this->pattern;
    }

    public function getMethod(): string {
        return $this->method;
    }

}

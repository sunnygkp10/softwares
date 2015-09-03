<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePatternAdapter;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern;
use APIs\Infrastructure\Objects\ConcreteHttpRoutePattern;

final class ConcreteHttpRoutePatternAdapter implements HttpRoutePatternAdapter {
    private $prefix;
    private $suffix;
    public function __construct(string $prefix, string $suffix) {
        $this->prefix = $prefix;
        $this->suffix = $suffix;
    }

    public function convertStringToHttpRoutePattern(string $pattern): HttpRoutePattern {
        return new ConcreteHttpRoutePattern($pattern, $this->prefix, $this->suffix);
    }

}

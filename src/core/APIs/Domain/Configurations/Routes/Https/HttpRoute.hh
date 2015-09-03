<?hh
namespace APIs\Domain\Configurations\Routes\Https;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern;

interface HttpRoute {
    public function getPattern(): HttpRoutePattern;
    public function getMethod(): string;
}

<?hh
namespace APIs\Applications\Routers;

interface HttpRouterApplication {
    public function route(string $uri, string $httpMethod): void;
}

<?hh
namespace APIs\Infrastructure\Applications;
use APIs\Infrastructure\Objects\ConcreteExecutableRESTAPI;
use APIs\Applications\RESTAPIs\RESTAPIApplication;
use APIs\Applications\Routers\HttpRouterApplication;
use APIs\Domain\API;

final class ConcreteHttpRouterApplication implements HttpRouterApplication {

    private $api;
    private $restAPIApplication;
    public function __construct(API $api, RESTAPIApplication $restAPIApplication) {
        $this->api = $api;
        $this->restAPIApplication = $restAPIApplication;
    }

    public function route(string $uri, string $httpMethod): void {

        $matches = array();
        $uri = strtok($uri, '?');
        preg_match_all('/\/([^\/]+)\/([^\/]+)/s', $uri, $matches);

        $httpMethod = strtolower($httpMethod);
        $routes = $this->api->getRoutes();
        foreach($routes as $oneRoute) {

            if (!$oneRoute->isHttpRoute()) {
                continue;
            }

            $httpRoute = $oneRoute->getHttpRoute();
            if ($httpRoute->getMethod() != $httpMethod) {
                continue;
            }

            $params = null;
            $restAPI = null;

            try {

                $params = $httpRoute->getPattern()->validate($uri);
                $restAPI = $oneRoute->getRESTAPI();

            } catch (\Exception $exception) {
                continue;
            }

            $executableRESTAPI = new ConcreteExecutableRESTAPI($restAPI, $params);
            echo $this->restAPIApplication->execute($executableRESTAPI);
            return;
        }

        throw new \Exception('There was no route found that matched the request.', 404);

    }

}

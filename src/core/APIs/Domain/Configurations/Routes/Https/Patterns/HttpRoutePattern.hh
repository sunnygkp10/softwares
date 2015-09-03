<?hh
namespace APIs\Domain\Configurations\Routes\Https\Patterns;

interface HttpRoutePattern {
    public function validate($uri): Map<string, string>;
    public function getPattern(): string;
    public function hasVariables(): boolean;
    public function getVariables(): ?Vector<string>;
    public function getVariablesWithDelimiters(): ?Vector<string>;
}

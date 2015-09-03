<?hh
namespace APIs\Domain\Configurations\Repositories;

interface RawConfigurationRepository {
    public function retrieve(string $url);
}

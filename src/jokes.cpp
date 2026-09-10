#include "jokes.hpp"
#include <random>
#include <chrono>
#include <unistd.h>

namespace FATfetch {

static const std::vector<std::string> JOKES_PL = {
    "Jak rozpoznać użytkownika Archa na imprezie? Nie musisz, sam ci powie w pierwszych 3 sekundach (po czym wróci do domu naprawiać Xorga).",
    "Użytkownik Archa wchodzi do prysznica... Żartowałem, użytkownicy Archa nie wiedzą co to prysznic.",
    "Ile zajmuje użytkownikowi Archa zmiana tapety? 4 dni: 3 dni na konfigurację Waybara w Hyprlandzie, 1 dzień na kłótnię na Reddicie o 'bloat'.",
    "Dlaczego użytkownik Archa nie ma dziewczyny? Bo 'Girlfriend' nie jest dostępna w oficjalnych repozytoriach, a wersja z AUR ma broken dependencies.",
    "- Kochanie, wyjdźmy na spacer, dotknijmy trawy!\n- 'Grass is bloat. Trawnik nie wspiera Waylanda.'",
    "Prawdziwy użytkownik Archa instaluje system nie po to, żeby go używać, tylko żeby móc odpalić neofetcha na Reddicie r/unixporn.",
    "Użytkownik Archa dzwoni po pomoc drogową:\n- Panie mechaniku, samochód nie działa!\n- A co pan robił?\n- Wpisałem `pacman -Syu --noconfirm` podczas jazdy.",
    "Co mówi użytkownik Archa po udanym seksie? Nic, budzi się i kompiluje kernel ze zoptymalizowanymi flagami -O3.",
    "Definicja minimalizmu według Archa: 450 pakietów z AUR, 6 GB animacji rozmycia w Hyprlandzie i 0 KB dezodorantu.",
    "Bóg stworzył świat w 6 dni. Siódmego dnia próbował zainstalować Archa bez archinstall i musiał czytać Arch Wiki przez kolejne 2000 lat.",
    "Dlaczego użytkownicy Archa noszą zakolanówki (programming socks)? Żeby nogi nie marzły podczas 8-godzinnej kompilacji przeglądarki ze źródeł.",
    "Zapytano użytkownika Archa o jego największe osiągnięcie życiowe:\n'Zmniejszyłem zużycie RAM-u o 4MB usuwając sterowniki do karty dźwiękowej'.",
    "Pukanie do drzwi:\n- Kto tam?\n- (Cisza)\n- Kto tam?!\n- BTW, I USE ARCH.",
    "Użytkownik Archa idzie do lekarza:\n- Panie doktorze, boli mnie głowa i mam gorączkę!\n- Próbował pan przeczytać Arch Wiki?",
    "Jaka jest różnica między użytkownikiem Ubuntu a Archa? Użytkownik Ubuntu używa komputera do pracy, użytkownik Archa pracuje nad używaniem komputera.",
    "Mama: 'Synu, dlaczego w twoim pokoju tak śmierdzi?'\nSyn: 'Mamo, to nie brud, to naturalny zapach wolnego oprogramowania i 12 dni bez wstawania od Hyprlanda!'",
    "Spotykają się dwaj użytkownicy Archa. Żaden nic nie mówi, po prostu pokazują sobie swoje konfiguracje Neovima i zaczynają płakać.",
    "Pytanie: Co jest cięższe - tona ołowiu czy przeciętny użytkownik Archa po 5 latach diety złożonej z Monsterków i paczek z AUR?\nOdpowiedź: Użytkownik Archa w rozmiarze 6XL.",
    "- Cześć, masz ogień?\n- Nie, ale mam Archa (btw) i mój laptop z procesorem kompilującym gentoo-kernel podpali ci papierosa w 2 sekundy.",
    "Jak brzmi modlitwa użytkownika Archa?\n'Pacmanie nasz, któryś jest w /usr/bin, święć się mirrory twoje, bądź wola twoja jako w AUR tak i na dysku...'",
    "Dlaczego w koszulkach z logo Archa robią tylko rozmiary od 4XL w górę? Bo prawdziwy szef społeczności nie mieści się w standardowe normy ISO.",
    "Co robi użytkownik Archa gdy dostanie awans w pracy? Zastanawia się jak zautomatyzować zwolnienie się za pomocą skryptu w bashu.",
    "Instrukcja obsługi życia użytkownika Archa:\n1. Bootuj z pendrive'a\n2. Zrób partycje fdiskiem\n3. Zapomnij zjeść obiad\n4. Spędź 10 lat na dopieszczaniu czcionki w terminalu.",
    "Status relacji użytkownika Archa: W skomplikowanym związku z `/etc/fstab` i błędem `Kernel panic - not syncing`.",
    "Ulubiony sport użytkownika Archa? Podnoszenie szklanki z Monsterkiem do ust i uciekanie przed odpowiedzialnością życiową.",
    "Co jest gorsze od błędu 404? Użytkownik Archa, który nie może ci powiedzieć, że używa Archa przez 5 minut.",
    "Użytkownik Archa wszedł kiedyś do lasu. Zobaczył drzewo i zaczął szukać pliku konfiguracyjnego `tree.conf`, żeby wyłączyć liście (bloat).",
    "Prawdziwy powód dla którego Arch nie ma domyślnego GUI: Twórcy wiedzieli, że i tak spędzisz 3 lata w tty1 szukając sensu życia."
};

static const std::vector<std::string> JOKES_EN = {
    "How do you know someone uses Arch Linux? Don't worry, they will tell you within 0.05 seconds of meeting you.",
    "An Arch user steps into the shower... Just kidding, Arch users don't know what a shower is.",
    "Why don't Arch Linux users have girlfriends? Because 'Girlfriend' isn't in official repos and the AUR version has broken dependencies.",
    "- 'Babe, let's go outside and touch some grass!'\n- 'Grass is bloat. The lawn doesn't support Wayland.'",
    "Arch Linux definition of minimalism: 500 packages from AUR, 8GB of Hyprland blur shaders, and 0 grams of deodorant.",
    "An Arch user calls roadside assistance:\n- 'My car won't start!'\n- 'What did you do?'\n- 'I ran pacman -Syu --noconfirm while driving on the highway.'",
    "What does an Arch user do after intimacy? Nothing, they wake up and compile a custom kernel with -O3 optimization flags.",
    "Knock knock.\nWho's there?\n(Silence)\nWho's there?!\n... BTW, I USE ARCH.",
    "How long does it take an Arch user to change a wallpaper? 4 days: 3 days ricing Waybar, 1 day arguing on Reddit about bloat.",
    "Why do Arch users wear programming thigh-high socks? To keep their legs warm during 12-hour Chromium builds from source.",
    "Asked about their greatest lifetime achievement:\n'I reduced idle RAM usage by 3.2MB by removing sound card drivers.'",
    "Arch user visits a doctor:\n- 'Doctor, my head hurts and I have a fever!'\n- 'Have you tried reading the Arch Wiki?'",
    "Difference between Ubuntu and Arch users: Ubuntu users use their OS to do work; Arch users work to use their OS.",
    "Mom: 'Why does your room smell like that?'\nSon: 'Mom, that is the natural fragrance of open-source freedom and 14 days without leaving Hyprland!'",
    "Two Arch users meet at a party. Neither says a word, they just show each other their Neovim dotfiles and start crying.",
    "Why are Arch Linux t-shirts only sold in sizes 4XL and above? Because true community leaders cannot be constrained by ISO standards.",
    "An Arch user walked into a forest, saw a tree, and tried to find `tree.conf` to disable leaves (bloat).",
    "Relationship status of an Arch user: In a complicated relationship with `/etc/fstab` and `Kernel panic - not syncing`.",
    "What is worse than a 404 error? An Arch user unable to say 'I use Arch btw' for 5 minutes.",
    "Arch user workout routine: 10 reps of lifting a Monster Energy can to mouth, 5 reps of arguing on Reddit.",
    "Why Arch Linux has no default GUI installer? Because developers knew you'd spend 3 years in tty1 searching for the meaning of life."
};

static const std::vector<std::string> ROASTS_PL = {
    "Twoje dotknięcie trawy wynosi dokładnie 0.000 ms.",
    "Ostatni prysznic: błąd wejścia/wyjścia (I/O error).",
    "Twój Neovim ma więcej wtyczek niż ty masz znajomych w realu.",
    "Kompilujesz kernel na baterii, żeby poczuć dreszczyk emocji.",
    "Twój rozmiar koszulki to Arch BTW XXXXL.",
    "Zużycie RAMu: 99% przez 40 kart z dokumentacją i Discordem.",
    "Prędzej Arch przejdzie na snap packages niż ty pójdziesz na siłownię."
};

static const std::vector<std::string> ROASTS_EN = {
    "Your grass contact time is precisely 0.000 ms.",
    "Last shower: Input/Output error (EIO).",
    "Your Neovim config has more plugins than you have friends in real life.",
    "You compile kernels on battery power just to feel something.",
    "Your shirt size is Arch BTW 6XL.",
    "RAM usage: 99% consumed by Discord and 50 Arch Wiki tabs.",
    "Arch will adopt snap packages before you step inside a gym."
};

static const std::vector<std::string> BELLY_COMMENTS_PL = {
    "350 kg czystego majestatu – służy jako ergonomiczna podstawka pod klawiaturę mechaniczną.",
    "Własne pole grawitacyjne: puste puszki po Monsterku krążą wokół pępka po orbicie geostacjonarnej.",
    "Pojemność: 4 kebaby z sosem mieszanym, 2 zgrzewki Monstera i 450 nieoczyszczonych pakietów z AUR.",
    "Zarejestrowany w urzędzie gminy jako budynek wielorodzinny o konstrukcji żelbetowo-tłuszczowej.",
    "Falowanie tłuszczu przy kichnięciu rejestrują stacje sejsmologiczne w Poczdamie (4.8 w skali Richtera).",
    "Rozmiar koszulki: Arch BTW XXXXXXL (szyta na zamówienie z plandeki od Żuka).",
    "Ciśnienie w bębnie: 8.5 bara. Jeden błąd w `pacman -Syu` i dochodzi do kontrolowanej eksplozji.",
    "Status bebecha: Wygiął metalowe podłokietniki w fotelu gamingowym pod kątem 45 stopni.",
    "Strefa bezwzględnego zakazu mydła i wody (certyfikat IP00 – totalny brak odporności na higienę).",
    "Rezonans akustyczny: bas z wentylatorów chłodzenia odbija się od brzucha dając dźwięk przestrzenny 7.1.",
    "Współczynnik oporu powietrza: Cx = 2.8 (aerodynamika lodówki polar z lat 80).",
    "Przy każdym kroku generuje falę uderzeniową wyłączającą sąsiadom Wi-Fi w promieniu 50 metrów.",
    "Podczas kompilacji jądra służy jako pasywny radiator termiczny odprowadzający 300W ciepła.",
    "Gdy kładziesz się na plecach, bebech całkowicie zasłania zakrzywiony monitor 34 cale ultrawide.",
    "Oficjalny sponsor: Kebab u Prawdziwego Turka i hurtownia energetyków bez cukru.",
    "Wskaźnik BMI: Wartość przekroczyła zakres 64-bitowej liczby całkowitej (integer overflow).",
    "Amortyzacja: Przeżył upadek z fotela obrotowego dzięki 18-centymetrowej warstwie ochronnego sadła.",
    "Lokalna strefa mikroklimatu: Wokół pępka panuje podwyższona wilgotność i naturalny aromat kebabu.",
    "Zastępuje biurko z regulacją wysokości – myszka i klawiatura mieszczą się na szczycie bębna.",
    "Certyfikat energetyczny: Klasa G+ (generuje tyle ciepła, że kot śpi na nim całą zimę).",
    "Grubość pancerza: 250mm stali pancernej z domieszką tłuszczów trans (odporny na pociski kumulacyjne).",
    "Status szwów w dresach: Stan krytyczny, naciąg na poziomie 99.8% wytrzymałości materiału.",
    "Zamiast kaloryfera masz potężny piec hutniczy zasilany pizzą i Monsterem Mango Loco.",
    "Poduszka powietrzna w aucie została zdemontowana, bo bebech wypełnia całą przestrzeń kabiny.",
    "W pasie ma więcej centymetrów niż Twój monitor gamingowy ma częstotliwości odświeżania w Hz.",
    "Gęstość materii w bębnie powoli zbliża się do gęstości gwiazdy neutronowej.",
    "Gdy zbliżasz się do lodówki, drzwi same się otwierają pod wpływem przyciągania masy tłuszczowej.",
    "Odgłos uderzenia dłonią w bęben: Dźwięk wielkiego bębna orkiestrowego niosący się po klatce schodowej.",
    "Zdolności pływackie: Bezwzględna niezatapialność – wyporność 380 litrów tłuszczu gwarantuje status boi morskiej.",
    "W trybie awaryjnym mieści zapasy kebabów wystarczające na przetrwanie 3 tygodni awarii internetu.",
    "Środek ciężkości znajduje się 40 centymetrów przed kolanami.",
    "Brak widoczności stóp od 2019 roku (stopy uznane za nieistotny bloat)."
};

static const std::vector<std::string> BELLY_COMMENTS_EN = {
    "350 kg of pure majesty – doubles as an ergonomic mechanical keyboard wrist rest.",
    "Has its own gravitational pull: empty Monster cans orbit the belly button in geostationary orbit.",
    "Storage capacity: 4 XL garlic kebabs, 2 cases of Monster Ultra, and 450 uncleaned AUR packages.",
    "Registered with local zoning authorities as a multi-family reinforced fat structure.",
    "Sneezing creates a seismic shockwave registered in Potsdam at 4.8 on the Richter scale.",
    "Shirt size: Arch BTW XXXXXXL (custom tailored from military surplus truck tarp).",
    "Internal pressure: 8.5 bar. One failed `pacman -Syu` away from controlled implosion.",
    "Status: Permanently bent the steel armrests of the gaming chair at a 45-degree angle.",
    "Certified IP00: Zero resistance to showers, maximum resistance to outdoor grass.",
    "Acoustic resonance: PC fan noise bounces off the belly producing natural 7.1 surround sound.",
    "Aerodynamic drag coefficient: Cx = 2.8 (aerodynamics of a 1980s Soviet refrigerator).",
    "Each step generates a kinetic tremor that knocks neighbor's 5GHz Wi-Fi offline.",
    "Acts as a massive passive thermal heatsink dissipating 300W during kernel compilation.",
    "Laying on your back completely blocks the line of sight to a 34-inch ultrawide monitor.",
    "Official sponsors: Local 24/7 Döner Kebab and wholesale energy drink distributors.",
    "BMI calculation resulted in an unsigned 64-bit integer overflow.",
    "Impact defense: Survived falling out of chair thanks to 7 inches of protective fat shielding.",
    "Microclimate warning: Localized high humidity and kebab scent detected near the navel.",
    "Replaces standing desks: Mouse and 60% keyboard easily sit atop the upper belly shelf.",
    "Thermal rating: Generates enough radiant heat that the cat sleeps on it all winter.",
    "Armor thickness: 250mm equivalent rolled homogenous fat armor.",
    "Sweatpant seams status: Critical structural stress, fabric tension at 99.8% yield limit.",
    "Car airbags removed: The belly already occupies all available cockpit volume.",
    "Waist circumference in centimeters exceeds your monitor refresh rate in Hz.",
    "Matter density in the belly core is rapidly approaching neutron star levels.",
    "Feet have not been observed visually since 2019 (feet declared deprecated bloat)."
};

static std::mt19937& getRng() {
    static std::random_device rd;
    static std::mt19937 rng(rd() ^ static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count() + getpid()));
    return rng;
}

std::string JokeGenerator::getRandomJoke(Language lang) {
    auto& rng = getRng();
    const auto& list = (lang == Language::PL) ? JOKES_PL : JOKES_EN;
    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
    return list[dist(rng)];
}

std::string JokeGenerator::getRandomRoast(Language lang) {
    auto& rng = getRng();
    const auto& list = (lang == Language::PL) ? ROASTS_PL : ROASTS_EN;
    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
    return list[dist(rng)];
}

std::string JokeGenerator::getRandomBellyComment(Language lang) {
    auto& rng = getRng();
    const auto& list = (lang == Language::PL) ? BELLY_COMMENTS_PL : BELLY_COMMENTS_EN;
    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
    return list[dist(rng)];
}

std::string JokeGenerator::getDietPlan(Language lang) {
    if (lang == Language::PL) {
        return 
            "\033[1;33m--- OFICJALNA DIETA UŻYTKOWNIKA ARCHA (FAT-TIER) ---\033[0m\n"
            " ┌────────────────────┬─────────────────────────────┬──────────┐\n"
            " │ Pora dnia          │ Posiłek                     │ Kalorie  │\n"
            " ├────────────────────┼─────────────────────────────┼──────────┤\n"
            " │ 14:00 (Śniadanie)  │ 2x Monster Mango Loco + AUR │ 850 kcal │\n"
            " │ 19:30 (Obiad)      │ Kebab XXL z sosem czosnkowym│ 1800 kcal│\n"
            " │ 03:15 (Kolacja)    │ Zimna pizza + pacman -Syu   │ 1200 kcal│\n"
            " │ 05:40 (Przekąska)  │ Paczka chipsów i łzy z sway │ 650 kcal │\n"
            " └────────────────────┴─────────────────────────────┴──────────┘\n"
            " \033[1;32mŁączny bilans:\033[0m 4500 kcal | \033[1;36mBloat Status:\033[0m MAKSYMALNY\n"
            " \033[1;35mAktywność fizyczna:\033[0m 420 kliknięć w klawiaturę mechaniczną (Blue switches)\n";
    } else {
        return 
            "\033[1;33m--- OFFICIAL ARCH LINUX CHAD DIET PLAN (FAT-TIER) ---\033[0m\n"
            " ┌────────────────────┬─────────────────────────────┬──────────┐\n"
            " │ Time of Day        │ Meal                        │ Calories │\n"
            " ├────────────────────┼─────────────────────────────┼──────────┤\n"
            " │ 14:00 (Breakfast)  │ 2x Monster Energy + AUR     │ 850 kcal │\n"
            " │ 19:30 (Lunch)      │ XXL Triple Cheesy Kebab     │ 1800 kcal│\n"
            " │ 03:15 (Dinner)     │ Cold Pizza + pacman -Syu    │ 1200 kcal│\n"
            " │ 05:40 (Late Snack) │ Bag of Doritos & Sway tears │ 650 kcal │\n"
            " └────────────────────┴─────────────────────────────┴──────────┘\n"
            " \033[1;32mTotal intake:\033[0m 4500 kcal | \033[1;36mBloat Status:\033[0m MAXIMUM\n"
            " \033[1;35mPhysical Activity:\033[0m 420 clicks on Clicky Blue Switch Mechanical Keyboard\n";
    }
}

} // namespace FATfetch

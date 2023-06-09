/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:40:32 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/14 16:24:46 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static int		observe_philosophers(t_observer *observer);
static void		check_philo_status(int philo, t_observer *observer);
static void		kill_philosopher(int philo, t_observer *observer);
static int		philo_eat_all(t_observer *observer);

void	*observer_routine(void *observer_data)
{
	t_observer	*observer;

	observer = (t_observer *)observer_data;
	while (TRUE)
	{
		if (philo_eat_all(observer))
			break ;
		if (!observe_philosophers(observer))
			break ;
	}
	return (NULL);
}

static int	observe_philosophers(t_observer *observer)
{
	int	philo;

	philo = 0;
	while (philo < observer->number_of_philosophers)
	{
		observer->current_time = get_time_now() - observer->start_time;
		check_philo_status(philo, observer);
		if (observer->current_time - observer->last_meal_time
			> observer->time_to_die)
		{
			if (observer->missing_meals[philo])
				kill_philosopher(philo, observer);
			return (0);
		}
		philo++;
	}
	return (1);
}

static int	philo_eat_all(t_observer *observer)
{
	int	philo;
	int	dones;

	philo = 0;
	dones = 0;
	while (philo < observer->number_of_philosophers)
	{
		if (!observer->missing_meals[philo])
			dones++;
		philo++;
	}
	if (dones == observer->number_of_philosophers)
	{
		pthread_mutex_lock(*observer->need_stop_mutex);
		*observer->need_stop = TRUE;
		pthread_mutex_unlock(*observer->need_stop_mutex);
		return (1);
	}
	return (0);
}

static void	check_philo_status(int philo, t_observer *observer)
{
	pthread_mutex_lock(observer->philo_status[philo]->status_mutex);
	observer->last_meal_time = observer->philo_status[philo]
		->last_meal_time;
	observer->missing_meals[philo]
		= observer->philo_status[philo]->missing_meals;
	pthread_mutex_unlock(observer->philo_status[philo]
		->status_mutex);
}

static void	kill_philosopher(int philo, t_observer *observer)
{
	pthread_mutex_lock(*observer->need_stop_mutex);
	*observer->need_stop = TRUE;
	pthread_mutex_unlock(*observer->need_stop_mutex);
	pthread_mutex_lock(*observer->print_mutex);
	printf("%d %d died\n", observer->current_time, philo + 1);
	pthread_mutex_unlock(*observer->print_mutex);
}
